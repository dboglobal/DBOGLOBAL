CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = 0;
	rq = 0;
	tid = 126;
	title = 12602;

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
				conv = 12609;
				ctype = 1;
				idx = 2801201;
				taid = 2;
			}
			CDboTSActRegQInfo
			{
				cont = 12608;
				gtype = 1;
				area = 12601;
				goal = 12604;
				grade = 132203;
				rwd = 100;
				scitem = -1;
				sort = 12605;
				stype = 2;
				taid = 1;
				title = 12602;
			}
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 12607;
			nolnk = 253;
			rm = 0;
			yeslnk = 1;

			CDboTSCheckLvl
			{
				maxlvl = 100;
				minlvl = 17;
			}
			CDboTSCheckPCRace
			{
				raceflg = 1;
			}
			CDboTSClickNPC
			{
				npcidx = "2801201;";
			}
			CDboTSCheckClrQst
			{
				flink = 1;
				flinknextqid = "127;";
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
			cont = 12608;
			gtype = 1;
			oklnk = 2;
			area = 12601;
			goal = 12604;
			sort = 12605;
			prelnk = "0;";
			ds = 1;
			grade = 132203;
			rwd = 100;
			title = 12602;
		}
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 12614;
			nextlnk = 254;
			rwdexp = 0;
			rwdtbl = 12601;
			rwdtype = 0;
			ltime = -1;
			prelnk = "2;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "4071110;";
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

