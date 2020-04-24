CNtlTSTrigger
{
	sm = 1;
	sq = 0;
	qc = 8;
	rq = 0;
	tid = 2410;
	title = 241002;

	CNtlTSGroup
	{
		gid = 0;

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
		CDboTSContGCond
		{
			cid = 5;
			prelnk = "4;";
			nolnk = 255;
			rm = 0;
			yeslnk = 6;

			CDboTSRcvSvrEvt
			{
				id = 504;
			}
		}
		CDboTSContGAct
		{
			cid = 6;
			elnk = 255;
			nextlnk = 100;
			prelnk = "5;";

			CDboTSActWorldPlayScript
			{
				sid = 2410;
				start = 0;
				taid = 1;
				uof = 0;
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
				sid = 2410;
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
				conv = 241009;
				ctype = 1;
				idx = 3132103;
				taid = 2;
			}
			CDboTSActRegQInfo
			{
				cont = 241008;
				gtype = 3;
				area = 241001;
				goal = 241004;
				grade = 132203;
				rwd = 100;
				scitem = -1;
				sort = 241005;
				stype = 1;
				taid = 1;
				title = 241002;
			}
		}
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 241014;
			nextlnk = 254;
			rwdexp = 0;
			rwdtbl = 241001;
			rwdtype = 0;
			ltime = -1;
			prelnk = "6;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "3148104;";
			}
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
			cont = 241008;
			gtype = 3;
			oklnk = 2;
			area = 241001;
			goal = 241004;
			sort = 241005;
			prelnk = "0;";
			ds = 1;
			grade = 0;
			rwd = 100;
			title = 241002;
		}
		CDboTSContEnd
		{
			cid = 253;
			prelnk = "1;0;252;";
			type = 0;
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 241007;
			nolnk = 253;
			rm = 0;
			yeslnk = 1;

			CDboTSCheckLvl
			{
				maxlvl = 100;
				minlvl = 52;
			}
			CDboTSCheckClrQst
			{
				and = "2408;";
				flink = 1;
				flinknextqid = "2411;";
				not = 0;
			}
			CDboTSClickNPC
			{
				npcidx = "3132103;";
			}
		}
	}
}

