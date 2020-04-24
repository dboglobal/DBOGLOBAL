CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = 0;
	rq = 0;
	tid = 154;
	title = 15402;

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
				conv = 15409;
				ctype = 1;
				idx = 3142203;
				taid = 2;
			}
			CDboTSActRegQInfo
			{
				cont = 15408;
				gtype = 1;
				area = 15401;
				goal = 15404;
				grade = 132203;
				rwd = 100;
				scitem = -1;
				sort = 15405;
				stype = 2;
				taid = 1;
				title = 15402;
			}
		}
		CDboTSContProposal
		{
			cancellnk = 253;
			cid = 1;
			cont = 15408;
			gtype = 1;
			oklnk = 2;
			area = 15401;
			goal = 15404;
			sort = 15405;
			prelnk = "0;";
			ds = 1;
			grade = 132203;
			rwd = 100;
			title = 15402;
		}
		CDboTSContGAct
		{
			cid = 252;
			elnk = 255;
			nextlnk = 253;
			prelnk = "2;";
		}
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 15414;
			nextlnk = 254;
			rwdexp = 0;
			rwdtbl = 15401;
			rwdtype = 0;
			ltime = -1;
			prelnk = "2;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "4071110;";
			}
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 15407;
			nolnk = 253;
			rm = 0;
			yeslnk = 1;

			CDboTSCheckLvl
			{
				maxlvl = 100;
				minlvl = 19;
			}
			CDboTSCheckPCCls
			{
				clsflg = 3;
			}
			CDboTSClickNPC
			{
				npcidx = "3142203;";
			}
			CDboTSCheckClrQst
			{
				and = "153;";
				flink = 1;
				flinknextqid = "155;";
				not = 0;
			}
			CDboTSCheckPCRace
			{
				raceflg = 1;
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

