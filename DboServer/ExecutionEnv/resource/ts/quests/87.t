CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = 0;
	rq = 0;
	tid = 87;
	title = 8702;

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
				conv = 8709;
				ctype = 1;
				idx = 6312102;
				taid = 2;
			}
			CDboTSActRegQInfo
			{
				cont = 8708;
				gtype = 1;
				area = 8701;
				goal = 8704;
				grade = 132203;
				rwd = 100;
				scitem = -1;
				sort = 8705;
				stype = 2;
				taid = 1;
				title = 8702;
			}
		}
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 8714;
			nextlnk = 254;
			rwdexp = 0;
			rwdtbl = 8701;
			rwdtype = 0;
			ltime = -1;
			prelnk = "2;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "1551102;";
			}
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
			prelnk = "0;1;252;";
			type = 0;
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
			cont = 8708;
			gtype = 1;
			oklnk = 2;
			area = 8701;
			goal = 8704;
			sort = 8705;
			prelnk = "0;";
			ds = 1;
			grade = 132203;
			rwd = 100;
			title = 8702;
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 8707;
			nolnk = 253;
			rm = 0;
			yeslnk = 1;

			CDboTSCheckPCRace
			{
				raceflg = 1;
			}
			CDboTSCheckPCCls
			{
				clsflg = 3;
			}
			CDboTSCheckLvl
			{
				maxlvl = 100;
				minlvl = 13;
			}
			CDboTSClickNPC
			{
				npcidx = "6312102;";
			}
			CDboTSCheckClrQst
			{
				flink = 1;
				flinknextqid = "93;";
				not = 0;
			}
		}
	}
}

