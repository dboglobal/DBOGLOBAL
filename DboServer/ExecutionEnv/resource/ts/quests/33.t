CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = 0;
	rq = 0;
	tid = 33;
	title = 3302;

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
				conv = 3309;
				ctype = 1;
				idx = 2951101;
				taid = 2;
			}
			CDboTSActRegQInfo
			{
				cont = 3308;
				gtype = 1;
				area = 3301;
				goal = 3304;
				grade = 132203;
				rwd = 100;
				scitem = -1;
				sort = 3305;
				stype = 1;
				taid = 1;
				title = 3302;
			}
		}
		CDboTSContProposal
		{
			cancellnk = 253;
			cid = 1;
			cont = 3308;
			gtype = 1;
			oklnk = 2;
			area = 3301;
			goal = 3304;
			sort = 3305;
			prelnk = "0;";
			ds = 1;
			grade = 132203;
			rwd = 100;
			title = 3302;
		}
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 3314;
			nextlnk = 254;
			rwdexp = 0;
			rwdtbl = 3301;
			rwdtype = 0;
			ltime = -1;
			prelnk = "2;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "5061201;";
			}
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
		CDboTSContEnd
		{
			cid = 253;
			prelnk = "0;252;1;";
			type = 0;
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 3307;
			nolnk = 253;
			rm = 0;
			yeslnk = 1;

			CDboTSCheckPCRace
			{
				raceflg = 1;
			}
			CDboTSCheckLvl
			{
				maxlvl = 100;
				minlvl = 5;
			}
			CDboTSCheckClrQst
			{
				flink = 1;
				flinknextqid = "34;";
				not = 0;
			}
			CDboTSClickNPC
			{
				npcidx = "2951101;";
			}
		}
	}
}

