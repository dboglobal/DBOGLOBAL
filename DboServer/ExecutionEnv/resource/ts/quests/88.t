CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = 0;
	rq = 0;
	tid = 88;
	title = 8802;

	CNtlTSGroup
	{
		gid = 0;

		CDboTSContGAct
		{
			cid = 252;
			elnk = 255;
			nextlnk = 253;
			prelnk = "2;";
		}
		CDboTSContGAct
		{
			cid = 2;
			elnk = 252;
			nextlnk = 100;
			prelnk = "1;";

			CDboTSActNPCConv
			{
				conv = 8809;
				ctype = 1;
				idx = 3143104;
				taid = 2;
			}
			CDboTSActRegQInfo
			{
				cont = 8808;
				gtype = 1;
				area = 8801;
				goal = 8804;
				grade = 132203;
				rwd = 100;
				scitem = -1;
				sort = 8805;
				stype = 1;
				taid = 1;
				title = 8802;
			}
		}
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 8814;
			nextlnk = 254;
			rwdexp = 0;
			rwdtbl = 8801;
			rwdtype = 0;
			ltime = -1;
			prelnk = "2;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "3183104;";
			}
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
			cont = 8808;
			gtype = 1;
			oklnk = 2;
			area = 8801;
			goal = 8804;
			sort = 8805;
			prelnk = "0;";
			ds = 1;
			grade = 132203;
			rwd = 100;
			title = 8802;
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 8807;
			nolnk = 253;
			rm = 0;
			yeslnk = 1;

			CDboTSCheckPCCls
			{
				clsflg = 1;
			}
			CDboTSCheckPCRace
			{
				raceflg = 1;
			}
			CDboTSCheckLvl
			{
				maxlvl = 100;
				minlvl = 13;
			}
			CDboTSClickNPC
			{
				npcidx = "3143104;";
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

