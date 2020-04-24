CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = 0;
	rq = 0;
	tid = 185;
	title = 18502;

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
				conv = 18509;
				ctype = 1;
				idx = 4041101;
				taid = 2;
			}
			CDboTSActRegQInfo
			{
				cont = 18508;
				gtype = 1;
				area = 18501;
				goal = 18504;
				grade = 132203;
				rwd = 100;
				scitem = -1;
				sort = 18505;
				stype = 2;
				taid = 1;
				title = 18502;
			}
		}
		CDboTSContProposal
		{
			cancellnk = 253;
			cid = 1;
			cont = 18508;
			gtype = 1;
			oklnk = 2;
			area = 18501;
			goal = 18504;
			sort = 18505;
			prelnk = "0;";
			ds = 1;
			grade = 132203;
			rwd = 100;
			title = 18502;
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 18507;
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
				minlvl = 20;
			}
			CDboTSClickNPC
			{
				npcidx = "4041101;";
			}
			CDboTSCheckClrQst
			{
				flink = 1;
				flinknextqid = "186;";
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
			desc = 18514;
			nextlnk = 254;
			rwdexp = 0;
			rwdtbl = 18501;
			rwdtype = 0;
			ltime = -1;
			prelnk = "2;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "5312101;";
			}
		}
	}
}

