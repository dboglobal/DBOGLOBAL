CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = 3;
	rq = 0;
	tid = 680;
	title = 68002;

	CNtlTSGroup
	{
		gid = 0;

		CDboTSContGCond
		{
			cid = 4;
			prelnk = "100;2;";
			nolnk = 255;
			rm = 1;
			yeslnk = 3;

			CDboTSClickNPC
			{
				npcidx = "3031101;";
			}
		}
		CDboTSContGAct
		{
			cid = 2;
			elnk = 252;
			nextlnk = 4;
			prelnk = "1;";

			CDboTSActNPCConv
			{
				conv = 68009;
				ctype = 1;
				idx = 3221201;
				taid = 2;
			}
			CDboTSActRegQInfo
			{
				cont = 68008;
				gtype = 1;
				area = 68001;
				goal = 68004;
				grade = 132203;
				rwd = 100;
				scitem = -1;
				sort = 68005;
				stype = 2;
				taid = 1;
				title = 68002;
			}
		}
		CDboTSContUnifiedNarration
		{
			cancellnk = 100;
			cid = 3;
			idx = 6803;
			lilnk = 255;
			oklnk = 100;
			prelnk = "4;";
		}
		CDboTSContReward
		{
			canclnk = 4;
			cid = 100;
			rwdzeny = 0;
			desc = 68014;
			nextlnk = 254;
			rwdexp = 0;
			rwdtbl = 68001;
			rwdtype = 0;
			ltime = -1;
			prelnk = "3;3;";
			usetbl = 1;
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
			cont = 68008;
			gtype = 1;
			oklnk = 2;
			area = 68001;
			goal = 68004;
			sort = 68005;
			prelnk = "0;";
			ds = 1;
			grade = 132203;
			rwd = 100;
			title = 68002;
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 68007;
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
				minlvl = 23;
			}
			CDboTSCheckClrQst
			{
				and = "679;";
				flink = 1;
				flinknextqid = "709;";
				not = 0;
			}
			CDboTSClickNPC
			{
				npcidx = "3221201;";
			}
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
	}
}

