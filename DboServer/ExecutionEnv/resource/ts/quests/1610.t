CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = 6;
	rq = 0;
	tid = 1610;
	title = 161002;

	CNtlTSGroup
	{
		gid = 0;

		CDboTSContGAct
		{
			cid = 2;
			elnk = 252;
			nextlnk = 100;
			prelnk = "1;";

			CDboTSActNPCConv
			{
				conv = 161009;
				ctype = 1;
				idx = 5061101;
				taid = 2;
			}
			CDboTSActRegQInfo
			{
				cont = 161008;
				gtype = 1;
				area = 161001;
				goal = 161004;
				grade = 132203;
				rwd = 100;
				scitem = -1;
				sort = 161005;
				stype = 2;
				taid = 1;
				title = 161002;
			}
		}
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 161014;
			nextlnk = 254;
			rwdexp = 0;
			rwdtbl = 161001;
			rwdtype = 0;
			ltime = -1;
			prelnk = "2;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "4071117;";
			}
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 161007;
			nolnk = 253;
			rm = 0;
			yeslnk = 1;

			CDboTSCheckLvl
			{
				maxlvl = 100;
				minlvl = 48;
			}
			CDboTSClickNPC
			{
				npcidx = "5061111;";
			}
			CDboTSCheckClrQst
			{
				and = "1599;";
				flink = 1;
				not = 0;
			}
		}
		CDboTSContProposal
		{
			cancellnk = 253;
			cid = 1;
			cont = 161008;
			gtype = 1;
			oklnk = 2;
			area = 161001;
			goal = 161004;
			sort = 161005;
			prelnk = "0;";
			ds = 1;
			grade = 132203;
			rwd = 100;
			title = 161002;
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
			prelnk = "2;";
		}
		CDboTSContEnd
		{
			cid = 254;
			prelnk = "100;";
			type = 1;
		}
	}
}

