CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = 3;
	rq = 0;
	tid = 863;
	title = 86302;

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
				conv = 86309;
				ctype = 1;
				idx = 3031101;
				taid = 2;
			}
			CDboTSActRegQInfo
			{
				cont = 86308;
				gtype = 1;
				area = 86301;
				goal = 86304;
				grade = 132203;
				rwd = 100;
				scitem = -1;
				sort = 86305;
				stype = 2;
				taid = 1;
				title = 86302;
			}
		}
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 86314;
			nextlnk = 254;
			rwdexp = 0;
			rwdtbl = 86301;
			rwdtype = 0;
			ltime = -1;
			prelnk = "2;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "1653101;";
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
			cont = 86308;
			gtype = 1;
			oklnk = 2;
			area = 86301;
			goal = 86304;
			sort = 86305;
			prelnk = "0;";
			ds = 1;
			grade = 132203;
			rwd = 100;
			title = 86302;
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 86307;
			nolnk = 253;
			rm = 0;
			yeslnk = 1;

			CDboTSCheckLvl
			{
				maxlvl = 100;
				minlvl = 30;
			}
			CDboTSClickNPC
			{
				npcidx = "3031101;";
			}
			CDboTSCheckClrQst
			{
				flink = 1;
				flinknextqid = "864;";
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
			prelnk = "100;";
			type = 1;
		}
	}
}

