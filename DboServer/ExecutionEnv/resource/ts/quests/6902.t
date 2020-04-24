CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = 23;
	rq = 0;
	tid = 6902;
	title = 690202;

	CNtlTSGroup
	{
		gid = 0;

		CDboTSContGAct
		{
			cid = 4;
			elnk = 255;
			nextlnk = 254;
			prelnk = "100;";

			CDboTSActSToCEvt
			{
				apptype = 0;
				cnt1 = 0;
				eitype = 1;
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
		}
		CDboTSContEnd
		{
			cid = 254;
			prelnk = "4;";
			type = 1;
		}
		CDboTSContEnd
		{
			cid = 253;
			prelnk = "2;1;0;";
			type = 0;
		}
		CDboTSContGAct
		{
			cid = 2;
			elnk = 253;
			nextlnk = 3;
			prelnk = "1;";

			CDboTSActNPCConv
			{
				conv = 690209;
				ctype = 1;
				idx = 3185102;
				taid = 3;
			}
			CDboTSActSToCEvt
			{
				apptype = 0;
				cnt1 = 0;
				eitype = 1;
				idx0 = 1613;
				cnt0 = 1;
				cnt2 = 0;
				ectype = -1;
				etype = 0;
				idx1 = -1;
				taid = 2;
				esctype = 0;
				idx2 = -1;
			}
			CDboTSActRegQInfo
			{
				cont = 690208;
				gtype = 1;
				area = 690201;
				goal = 690204;
				m0fz = "-563.750000";
				m0widx = 1;
				sort = 690205;
				stype = 1;
				m0fy = "-58.830002";
				m0pat = 1;
				grade = -1;
				m0fx = "4654.529785";
				m0ttip = 690215;
				rwd = 100;
				taid = 1;
				title = 690202;
			}
		}
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 690214;
			nextlnk = 4;
			rwdexp = 0;
			rwdtbl = 690201;
			rwdtype = 0;
			ltime = -1;
			prelnk = "3;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "3185102;";
			}
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 690207;
			nolnk = 253;
			rm = 0;
			yeslnk = 1;

			CDboTSCheckClrQst
			{
				and = "6888;";
				flink = 0;
				flinknextqid = "6903;";
				not = 0;
			}
			CDboTSClickNPC
			{
				npcidx = "3185102;";
			}
			CDboTSCheckPCRace
			{
				raceflg = 7;
			}
			CDboTSCheckLvl
			{
				maxlvl = 100;
				minlvl = 40;
			}
		}
		CDboTSContGCond
		{
			cid = 3;
			prelnk = "2;";
			nolnk = 255;
			rm = 0;
			yeslnk = 100;

			CDboTSCheckSToCEvt
			{
				itype = 1;
			}
		}
		CDboTSContProposal
		{
			cancellnk = 253;
			cid = 1;
			cont = 690208;
			gtype = 1;
			oklnk = 2;
			area = 690201;
			goal = 690204;
			sort = 690205;
			prelnk = "0;";
			ds = 1;
			grade = 0;
			rwd = 100;
			title = 690202;
		}
	}
	CNtlTSGroup
	{
		gid = 254;

		CDboTSContStart
		{
			cid = 0;
			stdiag = 690207;
			nolnk = 255;
			rm = 0;
			yeslnk = 1;
		}
		CDboTSContGAct
		{
			cid = 1;
			elnk = 255;
			nextlnk = 254;
			prelnk = "0;";

			CDboTSActQItem
			{
				icnt0 = 255;
				iprob0 = "1.000000";
				iidx0 = 1613;
				taid = 1;
				type = 1;
			}
		}
		CDboTSContEnd
		{
			cid = 254;
			prelnk = "1;";
			type = 0;
		}
	}
}

