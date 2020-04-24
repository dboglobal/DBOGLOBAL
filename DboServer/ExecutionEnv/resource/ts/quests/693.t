CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = 3;
	rq = 0;
	tid = 693;
	title = 69302;

	CNtlTSGroup
	{
		gid = 0;

		CDboTSContGAct
		{
			cid = 101;
			elnk = 255;
			nextlnk = 254;
			prelnk = "100;";

			CDboTSActSToCEvt
			{
				apptype = 0;
				cnt1 = 0;
				eitype = 2;
				idx0 = -1;
				cnt0 = 0;
				cnt2 = 0;
				ectype = -1;
				etype = 1;
				idx1 = -1;
				taid = 1;
				esctype = 0;
				idx2 = -1;
			}
			CDboTSActQItem
			{
				icnt0 = 255;
				iprob0 = "1.000000";
				iidx0 = 687;
				taid = 2;
				type = 1;
			}
		}
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 69314;
			nextlnk = 101;
			rwdexp = 0;
			rwdtbl = 69301;
			rwdtype = 0;
			ltime = -1;
			prelnk = "2;";
			usetbl = 1;

			CDboTSCheckQItem
			{
				ct = 0;
				icnt = 1;
				iidx = 687;
			}
			CDboTSClickNPC
			{
				npcidx = "3173101;";
			}
		}
		CDboTSContGAct
		{
			cid = 2;
			elnk = 252;
			nextlnk = 100;
			prelnk = "1;";

			CDboTSActNPCConv
			{
				conv = 69309;
				ctype = 1;
				idx = 3332201;
				taid = 2;
			}
			CDboTSActRegQInfo
			{
				cont = 69308;
				gtype = 1;
				area = 69301;
				goal = 69304;
				grade = 132203;
				rwd = 100;
				scitem = -1;
				sort = 69305;
				stype = 2;
				taid = 1;
				title = 69302;
			}
			CDboTSActSToCEvt
			{
				apptype = 0;
				cnt1 = 0;
				eitype = 2;
				idx0 = 687;
				cnt0 = 1;
				cnt2 = 0;
				ectype = -1;
				etype = 0;
				idx1 = -1;
				taid = 3;
				esctype = 0;
				idx2 = -1;
			}
		}
		CDboTSContEnd
		{
			cid = 254;
			prelnk = "101;";
			type = 1;
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
			cont = 69308;
			gtype = 1;
			oklnk = 2;
			area = 69301;
			goal = 69304;
			sort = 69305;
			prelnk = "0;";
			ds = 1;
			grade = 132203;
			rwd = 100;
			title = 69302;
		}
		CDboTSContEnd
		{
			cid = 253;
			prelnk = "0;1;252;";
			type = 0;
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 69307;
			nolnk = 253;
			rm = 0;
			yeslnk = 1;

			CDboTSCheckPCRace
			{
				raceflg = 4;
			}
			CDboTSCheckLvl
			{
				maxlvl = 100;
				minlvl = 23;
			}
			CDboTSClickNPC
			{
				npcidx = "3332201;";
			}
			CDboTSCheckClrQst
			{
				and = "692;";
				flink = 1;
				flinknextqid = "720;";
				not = 0;
			}
		}
	}
	CNtlTSGroup
	{
		gid = 254;

		CDboTSContGCond
		{
			cid = 1;
			prelnk = "0;";
			nolnk = 254;
			rm = 0;
			yeslnk = 2;

			CDboTSCheckQItem
			{
				ct = 1;
				icnt = 1;
				iidx = 687;
			}
		}
		CDboTSContEnd
		{
			cid = 254;
			prelnk = "2;1;";
			type = 0;
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 69307;
			nolnk = 255;
			rm = 0;
			yeslnk = 1;
		}
		CDboTSContGAct
		{
			cid = 2;
			elnk = 255;
			nextlnk = 254;
			prelnk = "1;";

			CDboTSActQItem
			{
				icnt0 = 255;
				iprob0 = "1.000000";
				iidx0 = 687;
				taid = 1;
				type = 1;
			}
		}
	}
}

