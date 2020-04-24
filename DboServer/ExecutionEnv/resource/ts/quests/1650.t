CNtlTSTrigger
{
	sm = 1;
	sq = 0;
	qc = 6;
	rq = 0;
	tid = 1650;
	title = 165002;

	CNtlTSGroup
	{
		gid = 0;

		CDboTSContGAct
		{
			cid = 6;
			elnk = 252;
			nextlnk = 100;
			prelnk = "5;";

			CDboTSActWorldPlayScript
			{
				sid = 1650;
				start = 0;
				taid = 1;
				uof = 1;
			}
		}
		CDboTSContGCond
		{
			cid = 5;
			prelnk = "4;";
			nolnk = 255;
			rm = 0;
			yeslnk = 6;

			CDboTSRcvSvrEvt
			{
				id = 364;
			}
		}
		CDboTSContGAct
		{
			cid = 4;
			elnk = 252;
			nextlnk = 5;
			prelnk = "3;";

			CDboTSActWorldPlayScript
			{
				sid = 1650;
				start = 1;
				taid = 1;
				uof = 1;
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
			cid = 2;
			elnk = 252;
			nextlnk = 3;
			prelnk = "1;";

			CDboTSActNPCConv
			{
				conv = 165009;
				ctype = 1;
				idx = 7135201;
				taid = 2;
			}
			CDboTSActRegQInfo
			{
				cont = 165008;
				gtype = 3;
				area = 165001;
				goal = 165004;
				grade = 132203;
				rwd = 100;
				scitem = -1;
				sort = 165005;
				stype = 1;
				taid = 1;
				title = 165002;
			}
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 165007;
			nolnk = 253;
			rm = 0;
			yeslnk = 1;

			CDboTSCheckLvl
			{
				maxlvl = 100;
				minlvl = 49;
			}
			CDboTSClickNPC
			{
				npcidx = "7135201;";
			}
		}
		CDboTSContEnd
		{
			cid = 253;
			prelnk = "0;1;252;";
			type = 0;
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
			cont = 165008;
			gtype = 3;
			oklnk = 2;
			area = 165001;
			goal = 165004;
			sort = 165005;
			prelnk = "0;";
			ds = 1;
			grade = 132203;
			rwd = 100;
			title = 165002;
		}
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 165014;
			nextlnk = 254;
			rwdexp = 0;
			rwdtbl = 165001;
			rwdtype = 0;
			ltime = -1;
			prelnk = "6;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "3143108;";
			}
		}
		CDboTSContGAct
		{
			cid = 252;
			elnk = 255;
			nextlnk = 253;
			prelnk = "6;4;2;";
		}
	}
}

