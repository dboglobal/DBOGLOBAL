CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = 0;
	rq = 0;
	tid = 12;
	title = 1202;

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
				conv = 1209;
				ctype = 1;
				idx = 4181101;
				taid = 2;
			}
			CDboTSActRegQInfo
			{
				cont = 1208;
				gtype = 1;
				area = 1201;
				goal = 1204;
				grade = 132203;
				rwd = 100;
				scitem = -1;
				sort = 1205;
				stype = 2;
				taid = 1;
				title = 1202;
			}
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 1207;
			nolnk = 253;
			rm = 0;
			yeslnk = 1;

			CDboTSCheckLvl
			{
				maxlvl = 100;
				minlvl = 4;
			}
			CDboTSCheckPCRace
			{
				raceflg = 1;
			}
			CDboTSCheckPCCls
			{
				clsflg = 3;
			}
			CDboTSClickNPC
			{
				npcidx = "4181101;";
			}
			CDboTSCheckClrQst
			{
				flink = 1;
				flinknextqid = "13;";
				not = 0;
			}
		}
		CDboTSContEnd
		{
			cid = 253;
			prelnk = "0;1;252;";
			type = 0;
		}
		CDboTSContProposal
		{
			cancellnk = 253;
			cid = 1;
			cont = 1208;
			gtype = 1;
			oklnk = 2;
			area = 1201;
			goal = 1204;
			sort = 1205;
			prelnk = "0;";
			ds = 1;
			grade = 132203;
			rwd = 100;
			title = 1202;
		}
		CDboTSContEnd
		{
			cid = 254;
			prelnk = "100;";
			type = 1;
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
			desc = 1214;
			nextlnk = 254;
			rwdexp = 0;
			rwdtbl = 1201;
			rwdtype = 0;
			ltime = -1;
			prelnk = "2;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "3183103;";
			}
		}
	}
}

