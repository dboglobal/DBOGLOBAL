CNtlTSTrigger
{
	sm = 1;
	sq = 0;
	qc = 4;
	rq = 0;
	tid = 3756;
	title = 375602;

	CNtlTSGroup
	{
		gid = 0;

		CDboTSContStart
		{
			cid = 0;
			stdiag = 375607;
			nolnk = 253;
			rm = 0;
			yeslnk = 1;

			CDboTSCheckLvl
			{
				maxlvl = 100;
				minlvl = 60;
			}
			CDboTSClickNPC
			{
				npcidx = "3191101;";
			}
			CDboTSCheckClrQst
			{
				and = "3755;";
				flink = 1;
				not = 0;
			}
		}
		CDboTSContGAct
		{
			cid = 7;
			elnk = 255;
			nextlnk = 100;
			prelnk = "6;";

			CDboTSActWorldPlayScript
			{
				sid = 3756;
				start = 0;
				taid = 1;
				uof = 0;
			}
		}
		CDboTSContGCond
		{
			cid = 6;
			prelnk = "4;";
			nolnk = 255;
			rm = 0;
			yeslnk = 7;

			CDboTSRcvSvrEvt
			{
				id = 1200;
			}
		}
		CDboTSContGAct
		{
			cid = 3;
			elnk = 255;
			nextlnk = 4;
			prelnk = "2;";

			CDboTSActTWaitTS
			{
				taid = 1;
				time = 3000;
			}
		}
		CDboTSContGAct
		{
			cid = 4;
			elnk = 252;
			nextlnk = 6;
			prelnk = "3;";

			CDboTSActWorldPlayScript
			{
				sid = 3756;
				start = 1;
				taid = 1;
				uof = 1;
			}
		}
		CDboTSContGAct
		{
			cid = 2;
			elnk = 252;
			nextlnk = 3;
			prelnk = "1;";

			CDboTSActNPCConv
			{
				conv = 375609;
				ctype = 1;
				idx = 3191101;
				taid = 2;
			}
			CDboTSActRegQInfo
			{
				cont = 375608;
				gtype = 2;
				area = 375601;
				goal = 375604;
				grade = 132203;
				rwd = 100;
				scitem = -1;
				sort = 375605;
				stype = 2;
				taid = 1;
				title = 375602;
			}
		}
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 375614;
			nextlnk = 254;
			rwdexp = 0;
			rwdtbl = 375601;
			rwdtype = 0;
			ltime = -1;
			prelnk = "7;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "3191101;";
			}
		}
		CDboTSContEnd
		{
			cid = 253;
			prelnk = "0;1;252;";
			type = 0;
		}
		CDboTSContGAct
		{
			cid = 252;
			elnk = 255;
			nextlnk = 253;
			prelnk = "4;2;";
		}
		CDboTSContEnd
		{
			cid = 254;
			prelnk = "100;";
			type = 1;
		}
		CDboTSContProposal
		{
			cancellnk = 253;
			cid = 1;
			cont = 375608;
			gtype = 2;
			oklnk = 2;
			area = 375601;
			goal = 375604;
			sort = 375605;
			prelnk = "0;";
			ds = 1;
			grade = 132203;
			rwd = 100;
			title = 375602;
		}
	}
}

