CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = 5;
	rq = 0;
	tid = 1243;
	title = 124302;

	CNtlTSGroup
	{
		gid = 0;

		CDboTSContEnd
		{
			cid = 200;
			prelnk = "101;";
			type = 1;
		}
		CDboTSContGAct
		{
			cid = 101;
			elnk = 200;
			nextlnk = 254;
			prelnk = "100;";

			CDboTSActWPSFD
			{
				sid = 1243;
				taid = 1;
			}
		}
		CDboTSContGCond
		{
			cid = 3;
			prelnk = "2;";
			nolnk = 255;
			rm = 0;
			yeslnk = 100;

			CDboTSClickNPC
			{
				npcidx = "5042101;";
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
				conv = 124309;
				ctype = 1;
				idx = 4111101;
				taid = 2;
			}
			CDboTSActRegQInfo
			{
				cont = 124308;
				gtype = 1;
				area = 124301;
				goal = 124304;
				grade = 132203;
				rwd = 100;
				scitem = -1;
				sort = 124305;
				stype = 2;
				taid = 1;
				title = 124302;
			}
		}
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 124314;
			nextlnk = 101;
			rwdexp = 0;
			rwdtbl = 124301;
			rwdtype = 0;
			ltime = -1;
			prelnk = "3;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "5042101;";
			}
		}
		CDboTSContGAct
		{
			cid = 252;
			elnk = 255;
			nextlnk = 253;
			prelnk = "2;";
		}
		CDboTSContProposal
		{
			cancellnk = 253;
			cid = 1;
			cont = 124308;
			gtype = 1;
			oklnk = 2;
			area = 124301;
			goal = 124304;
			sort = 124305;
			prelnk = "0;";
			ds = 1;
			grade = 132203;
			rwd = 100;
			title = 124302;
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 124307;
			nolnk = 253;
			rm = 0;
			yeslnk = 1;

			CDboTSCheckLvl
			{
				maxlvl = 100;
				minlvl = 42;
			}
			CDboTSClickNPC
			{
				npcidx = "4111101;";
			}
			CDboTSCheckClrQst
			{
				flink = 1;
				flinknextqid = "1244;";
				not = 0;
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
			prelnk = "101;";
			type = 1;
		}
	}
}

