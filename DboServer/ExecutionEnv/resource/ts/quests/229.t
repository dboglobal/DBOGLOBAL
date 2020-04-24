CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = 1;
	rq = 0;
	tid = 229;
	title = 22902;

	CNtlTSGroup
	{
		gid = 0;

		CDboTSContGCond
		{
			cid = 5;
			prelnk = "100;2;";
			nolnk = 255;
			rm = 1;
			yeslnk = 4;

			CDboTSClickNPC
			{
				npcidx = "4651102;";
			}
		}
		CDboTSContUnifiedNarration
		{
			cancellnk = 100;
			cid = 4;
			idx = 2293;
			lilnk = 255;
			oklnk = 100;
			prelnk = "5;";
		}
		CDboTSContUnifiedNarration
		{
			cancellnk = 1;
			cid = 3;
			idx = 2291;
			lilnk = 255;
			oklnk = 1;
			prelnk = "0;";
		}
		CDboTSContGAct
		{
			cid = 2;
			elnk = 252;
			nextlnk = 5;
			prelnk = "1;";

			CDboTSActRegQInfo
			{
				cont = 22908;
				gtype = 1;
				area = 22901;
				goal = 22904;
				grade = 132203;
				rwd = 100;
				scitem = -1;
				sort = 22905;
				stype = 2;
				taid = 1;
				title = 22902;
			}
			CDboTSActNPCConv
			{
				conv = 22909;
				ctype = 1;
				idx = 4511301;
				taid = 2;
			}
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 22907;
			nolnk = 253;
			rm = 0;
			yeslnk = 3;

			CDboTSCheckPCRace
			{
				raceflg = 2;
			}
			CDboTSCheckLvl
			{
				maxlvl = 100;
				minlvl = 6;
			}
			CDboTSClickNPC
			{
				npcidx = "4511301;";
			}
			CDboTSCheckClrQst
			{
				flink = 1;
				flinknextqid = "230;";
				not = 0;
			}
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
			cont = 22908;
			gtype = 1;
			oklnk = 2;
			area = 22901;
			goal = 22904;
			sort = 22905;
			prelnk = "3;3;";
			ds = 0;
			grade = 132203;
			rwd = 100;
			title = 22902;
		}
		CDboTSContReward
		{
			canclnk = 5;
			cid = 100;
			rwdzeny = 0;
			desc = 22914;
			nextlnk = 254;
			rwdexp = 0;
			rwdtbl = 22901;
			rwdtype = 0;
			ltime = -1;
			prelnk = "4;4;";
			usetbl = 1;
		}
		CDboTSContEnd
		{
			cid = 254;
			prelnk = "100;";
			type = 1;
		}
	}
}

