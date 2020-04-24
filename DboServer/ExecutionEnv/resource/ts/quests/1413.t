CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = 5;
	rq = 0;
	tid = 1413;
	title = 141302;

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
				conv = 141309;
				ctype = 1;
				idx = 8032104;
				taid = 2;
			}
			CDboTSActRegQInfo
			{
				cont = 141308;
				gtype = 1;
				area = 141301;
				goal = 141304;
				grade = 132203;
				rwd = 100;
				scitem = -1;
				sort = 141305;
				stype = 1;
				taid = 1;
				title = 141302;
			}
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 141307;
			nolnk = 253;
			rm = 0;
			yeslnk = 1;

			CDboTSCheckLvl
			{
				maxlvl = 100;
				minlvl = 45;
			}
			CDboTSClickNPC
			{
				npcidx = "8032104;";
			}
			CDboTSCheckClrQst
			{
				flink = 1;
				flinknextqid = "1414;";
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
		CDboTSContProposal
		{
			cancellnk = 253;
			cid = 1;
			cont = 141308;
			gtype = 1;
			oklnk = 2;
			area = 141301;
			goal = 141304;
			sort = 141305;
			prelnk = "0;";
			ds = 1;
			grade = 132203;
			rwd = 100;
			title = 141302;
		}
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 141314;
			nextlnk = 254;
			rwdexp = 0;
			rwdtbl = 141301;
			rwdtype = 0;
			ltime = -1;
			prelnk = "2;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "1431105;";
			}
		}
		CDboTSContGAct
		{
			cid = 252;
			elnk = 255;
			nextlnk = 253;
			prelnk = "2;";
		}
	}
}

