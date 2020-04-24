CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = 3;
	rq = 0;
	tid = 853;
	title = 85302;

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
				conv = 85309;
				ctype = 1;
				idx = 1311205;
				taid = 2;
			}
			CDboTSActRegQInfo
			{
				cont = 85308;
				gtype = 1;
				area = 85301;
				goal = 85304;
				grade = 132203;
				rwd = 100;
				scitem = -1;
				sort = 85305;
				stype = 2;
				taid = 1;
				title = 85302;
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
			cont = 85308;
			gtype = 1;
			oklnk = 2;
			area = 85301;
			goal = 85304;
			sort = 85305;
			prelnk = "0;";
			ds = 1;
			grade = 132203;
			rwd = 100;
			title = 85302;
		}
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 85314;
			nextlnk = 254;
			rwdexp = 0;
			rwdtbl = 85301;
			rwdtype = 0;
			ltime = -1;
			prelnk = "2;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "3181201;";
			}
		}
		CDboTSContEnd
		{
			cid = 254;
			prelnk = "100;";
			type = 1;
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 85307;
			nolnk = 253;
			rm = 0;
			yeslnk = 1;

			CDboTSCheckLvl
			{
				maxlvl = 100;
				minlvl = 29;
			}
			CDboTSCheckClrQst
			{
				and = "852;";
				flink = 1;
				flinknextqid = "854;";
				not = 0;
			}
			CDboTSClickNPC
			{
				npcidx = "1311205;";
			}
		}
		CDboTSContEnd
		{
			cid = 253;
			prelnk = "0;1;252;";
			type = 0;
		}
	}
}

