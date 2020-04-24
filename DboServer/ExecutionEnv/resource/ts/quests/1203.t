CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = 5;
	rq = 0;
	tid = 1203;
	title = 120302;

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
				conv = 120309;
				ctype = 1;
				idx = 4141105;
				taid = 2;
			}
			CDboTSActRegQInfo
			{
				cont = 120308;
				gtype = 1;
				area = 120301;
				goal = 120304;
				grade = 132203;
				rwd = 100;
				scitem = -1;
				sort = 120305;
				stype = 1;
				taid = 1;
				title = 120302;
			}
		}
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 120314;
			nextlnk = 254;
			rwdexp = 0;
			rwdtbl = 120301;
			rwdtype = 0;
			ltime = -1;
			prelnk = "2;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "1351107;";
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
			cont = 120308;
			gtype = 1;
			oklnk = 2;
			area = 120301;
			goal = 120304;
			sort = 120305;
			prelnk = "0;";
			ds = 1;
			grade = 132203;
			rwd = 100;
			title = 120302;
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 120307;
			nolnk = 253;
			rm = 0;
			yeslnk = 1;

			CDboTSCheckLvl
			{
				maxlvl = 100;
				minlvl = 41;
			}
			CDboTSClickNPC
			{
				npcidx = "4141105;";
			}
			CDboTSCheckClrQst
			{
				and = "1202;";
				flink = 1;
				flinknextqid = "1204;";
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

